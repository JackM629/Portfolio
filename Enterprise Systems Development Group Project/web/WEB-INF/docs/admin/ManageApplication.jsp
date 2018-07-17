<%-- 
    Document   : ManageApplication
    Created on : 27-Nov-2017, 13:16:36
    Author     : Frazer, Sasha
--%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<%@ include file="/resources/AdminDashboardNavBar.jsp" %>

<div class="content">
    <h1>Manage Application for ${requestedMember[1]}</h1> 

    <table>
        <tr>
            <th>Member ID</th>
            <th>Name</th>
            <th>Address</th>
            <th>DOB</th>
            <th>DOR</th>
            <th>Membership</th>
            <th>Balance</th>
        </tr>
        <tr>
            <c:forEach items="${requestedMember}" var="column" varStatus="columnStatus">
                <c:choose>
                    <c:when test="${columnStatus.last}">
                        <td>£${column}</td>
                    </c:when>
                    <c:otherwise>
                        <td>${column}</td>
                    </c:otherwise>
                </c:choose>
            </c:forEach>
        </tr>
    </table>
    <br><br>

    <form action="${pageContext.request.contextPath}/AdminController" method="post">
        <h2>Payments</h2>
        <table>
            <tr>
                <th>Type</th>
                <th>Amount</th>
                <th>Date</th>
                <th>Time</th> 
            </tr>
            <c:forEach items="${requestedPayments}" var="row" varStatus="rowStatus">
                <tr>
                    <c:forEach items="${row}" var="column" varStatus="columnStatus">
                        <c:choose>
                            <c:when test="${columnStatus.index eq 3}">
                                <td>£${column}</td>
                            </c:when>
                            <c:otherwise>
                                <c:if test="${columnStatus.index ne 0 and columnStatus.index ne 1}">
                                    <td>${column}</td>
                                </c:if>
                            </c:otherwise>
                        </c:choose>
                    </c:forEach>
                </tr>
            </c:forEach>
        </table>
        <br><br>
        <input type="hidden" name="viewId" value="/ManageApplication">
        <button class="btn" type="submit" name="manageMemberAction" value="approve_${requestedMember[0]}">Approve Application</button>
    </form> 
</div>
